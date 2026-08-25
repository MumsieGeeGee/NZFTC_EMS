(() => {
    const warmedUrls = new Set();

    function prefetchPage(url) {
        if (!url || warmedUrls.has(url)) {
            return;
        }

        warmedUrls.add(url);

        const prefetchLink = document.createElement("link");
        prefetchLink.rel = "prefetch";
        prefetchLink.as = "document";
        prefetchLink.href = url;
        document.head.appendChild(prefetchLink);

        fetch(url, {
            method: "GET",
            credentials: "same-origin"
        }).catch(() => {
            // Best effort prefetch only.
        });
    }

    function wireSidebarPrefetch() {
        const links = document.querySelectorAll(".left-sidebar a.sidebar-button[href]");
        links.forEach((link) => {
            const url = link.getAttribute("href");
            if (!url) {
                return;
            }

            const triggerPrefetch = () => prefetchPage(url);
            link.addEventListener("mouseenter", triggerPrefetch, { once: true });
            link.addEventListener("focus", triggerPrefetch, { once: true });
            link.addEventListener("touchstart", triggerPrefetch, { once: true, passive: true });
        });
    }

    if (document.readyState === "loading") {
        document.addEventListener("DOMContentLoaded", wireSidebarPrefetch, { once: true });
    } else {
        wireSidebarPrefetch();
    }
})();
