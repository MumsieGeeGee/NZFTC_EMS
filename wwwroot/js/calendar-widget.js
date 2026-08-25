(function () {
    function formatMonthLabel(year, monthIndex) {
        const firstDay = new Date(year, monthIndex, 1);
        const startColumn = (firstDay.getDay() + 6) % 7;
        const daysInMonth = new Date(year, monthIndex + 1, 0).getDate();
        const occupied = startColumn + daysInMonth;
        const spills = occupied > 35;

        const monthName = firstDay.toLocaleString("en-NZ", { month: "long" });
        if (!spills) {
            return `${monthName} ${year}`;
        }

        const nextMonthDate = new Date(year, monthIndex + 1, 1);
        const nextMonthName = nextMonthDate.toLocaleString("en-NZ", { month: "long" });
        const nextMonthYear = nextMonthDate.getFullYear();
        if (nextMonthYear !== year) {
            return `${monthName}/${nextMonthName} ${year}/${nextMonthYear}`;
        }
        return `${monthName}/${nextMonthName} ${year}`;
    }

    function createDayCell(dayNumber, year, monthNumber, holidayMap, isSpillover) {
        const cell = document.createElement("td");
        const wrapper = document.createElement("div");
        wrapper.className = "calendar-day-cell has-day";

        const badge = document.createElement("span");
        badge.className = isSpillover ? "day-badge spillover" : "day-badge";
        badge.textContent = String(dayNumber);
        wrapper.appendChild(badge);

        const eventBars = document.createElement("div");
        eventBars.className = "event-bars";

        const key = `${year}-${String(monthNumber).padStart(2, "0")}-${String(dayNumber).padStart(2, "0")}`;
        if (holidayMap.has(key)) {
            const holidayNames = holidayMap.get(key);
            const bar = document.createElement("div");
            bar.className = "event-bar";
            eventBars.appendChild(bar);
            wrapper.title = Array.isArray(holidayNames) ? holidayNames.join(", ") : "";
        }

        wrapper.appendChild(eventBars);
        cell.appendChild(wrapper);
        return cell;
    }

    function createEmptyCell() {
        const cell = document.createElement("td");
        const wrapper = document.createElement("div");
        wrapper.className = "calendar-day-cell";
        cell.appendChild(wrapper);
        return cell;
    }

    function buildCalendarTable(year, monthIndex, holidayMap) {
        const firstDay = new Date(year, monthIndex, 1);
        const firstDayWeekIndex = (firstDay.getDay() + 6) % 7; // Monday start
        const daysInMonth = new Date(year, monthIndex + 1, 0).getDate();

        const table = document.createElement("table");
        table.className = "calendar-table";
        const headRow = document.createElement("tr");
        ["Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"].forEach((name) => {
            const th = document.createElement("th");
            th.textContent = name;
            headRow.appendChild(th);
        });
        const thead = document.createElement("thead");
        thead.appendChild(headRow);
        table.appendChild(thead);

        const tbody = document.createElement("tbody");
        let dayCounter = 1;
        let nextMonthDay = 1;
        const nextMonthDate = new Date(year, monthIndex + 1, 1);
        const nextMonthNumber = nextMonthDate.getMonth() + 1;
        const nextMonthYear = nextMonthDate.getFullYear();

        for (let rowIndex = 0; rowIndex < 6; rowIndex++) {
            const row = document.createElement("tr");
            for (let colIndex = 0; colIndex < 7; colIndex++) {
                const cellIndex = rowIndex * 7 + colIndex;

                if (cellIndex >= firstDayWeekIndex && dayCounter <= daysInMonth) {
                    row.appendChild(createDayCell(dayCounter, year, monthIndex + 1, holidayMap, false));
                    dayCounter++;
                } else if (dayCounter > daysInMonth) {
                    row.appendChild(createDayCell(nextMonthDay, nextMonthYear, nextMonthNumber, holidayMap, true));
                    nextMonthDay++;
                } else {
                    row.appendChild(createEmptyCell());
                }
            }
            tbody.appendChild(row);
        }

        table.appendChild(tbody);
        return table;
    }

    function normalizeScope(scope) {
        if (!scope) {
            return "personal-only";
        }
        return scope.replace(/-/g, " ");
    }

    async function fetchCalendarData(host, year) {
        const endpoint = host.dataset.calendarEndpoint;
        if (!endpoint) {
            return { scope: "personal-only", holidayMap: new Map() };
        }

        const response = await fetch(`${endpoint}?year=${year}`, { credentials: "same-origin" });
        if (!response.ok) {
            throw new Error(`Calendar endpoint failed with status ${response.status}`);
        }

        const payload = await response.json();
        const holidayMap = new Map();
        const publicHolidays = Array.isArray(payload.publicHolidays) ? payload.publicHolidays : [];
        publicHolidays.forEach((holiday) => {
            if (!holiday || !holiday.date || !holiday.name) {
                return;
            }

            if (!holidayMap.has(holiday.date)) {
                holidayMap.set(holiday.date, []);
            }
            holidayMap.get(holiday.date).push(holiday.name);
        });

        return {
            scope: normalizeScope(payload.scope),
            holidayMap
        };
    }

    async function ensureYearData(host, year, state) {
        if (state.holidayMaps.has(year)) {
            return;
        }

        const data = await fetchCalendarData(host, year);
        state.scope = data.scope;
        state.holidayMaps.set(year, data.holidayMap);
    }

    function mergeHolidayMaps(primaryMap, secondaryMap) {
        const merged = new Map();
        [primaryMap, secondaryMap].forEach((map) => {
            if (!map) {
                return;
            }

            map.forEach((value, key) => {
                if (!merged.has(key)) {
                    merged.set(key, []);
                }
                merged.get(key).push(...value);
            });
        });
        return merged;
    }

    function render(host, state) {
        const monthLabel = host.querySelector("[data-calendar-month-label]");
        const scopeLabel = host.querySelector("[data-calendar-scope]");
        const gridHost = host.querySelector("[data-calendar-grid]");
        const prevButton = host.querySelector("[data-calendar-prev]");
        if (!monthLabel || !scopeLabel || !gridHost || !prevButton) {
            return;
        }

        const spilloverYear = new Date(state.currentYear, state.currentMonthIndex + 1, 1).getFullYear();
        const holidayMap = mergeHolidayMaps(
            state.holidayMaps.get(state.currentYear),
            state.holidayMaps.get(spilloverYear)
        );

        monthLabel.textContent = formatMonthLabel(state.currentYear, state.currentMonthIndex);
        scopeLabel.textContent = `Calendar scope: ${state.scope}`;
        prevButton.disabled =
            state.currentYear === state.baseYear &&
            state.currentMonthIndex === state.baseMonthIndex;

        gridHost.innerHTML = "";
        gridHost.appendChild(buildCalendarTable(state.currentYear, state.currentMonthIndex, holidayMap));
    }

    async function initWidget(host) {
        const today = new Date();
        const state = {
            baseYear: today.getFullYear(),
            baseMonthIndex: today.getMonth(),
            currentYear: today.getFullYear(),
            currentMonthIndex: today.getMonth(),
            holidayMaps: new Map(),
            scope: "personal-only"
        };

        async function refreshMonth() {
            await ensureYearData(host, state.currentYear, state);
            const spilloverYear = new Date(state.currentYear, state.currentMonthIndex + 1, 1).getFullYear();
            await ensureYearData(host, spilloverYear, state);
            render(host, state);
        }

        function stepMonth(step) {
            const nextMonthIndex = state.currentMonthIndex + step;
            state.currentYear += Math.floor(nextMonthIndex / 12);
            state.currentMonthIndex = ((nextMonthIndex % 12) + 12) % 12;
        }

        const prevButton = host.querySelector("[data-calendar-prev]");
        const nextButton = host.querySelector("[data-calendar-next]");

        if (prevButton) {
            prevButton.addEventListener("click", async () => {
                const atBaseMonth =
                    state.currentYear === state.baseYear &&
                    state.currentMonthIndex === state.baseMonthIndex;
                if (atBaseMonth) {
                    return;
                }

                stepMonth(-1);
                await refreshMonth();
            });
        }

        if (nextButton) {
            nextButton.addEventListener("click", async () => {
                stepMonth(1);
                await refreshMonth();
            });
        }

        try {
            await refreshMonth();
        } catch (error) {
            console.error(error);
            render(host, state);
        }
    }

    document.addEventListener("DOMContentLoaded", function () {
        document.querySelectorAll("[data-calendar-endpoint]").forEach((host) => {
            initWidget(host);
        });
    });
})();
