(() => {
  'use strict';
  const isZh = document.documentElement.lang === 'zh-CN';
  const openLabel = isZh ? '展开导航' : 'Open navigation';
  const closeLabel = isZh ? '收起导航' : 'Close navigation';
  const nav = document.querySelector('.global-nav');
  const menu = document.querySelector('#site-nav');
  const toggle = document.querySelector('.menu-toggle');
  const reduced = matchMedia('(prefers-reduced-motion: reduce)');
  function closeMenu() {
    menu.classList.remove('is-open');
    toggle.setAttribute('aria-expanded', 'false');
    toggle.setAttribute('aria-label', openLabel);
  }
  toggle.addEventListener('click', () => {
    const open = toggle.getAttribute('aria-expanded') !== 'true';
    toggle.setAttribute('aria-expanded', String(open));
    toggle.setAttribute('aria-label', open ? closeLabel : openLabel);
    menu.classList.toggle('is-open', open);
  });
  menu.addEventListener('click', event => { if (event.target.closest('a')) closeMenu(); });
  document.addEventListener('keydown', event => {
    if (event.key === 'Escape' && toggle.getAttribute('aria-expanded') === 'true') { closeMenu(); toggle.focus(); }
  });
  matchMedia('(min-width: 761px)').addEventListener('change', closeMenu);
  const reveals = [...document.querySelectorAll('.section-heading,.silicon-heading,.silicon-portrait,.stack-card,.capability-table,.application-grid>a,.audience-card,.roadmap-version,.news-feature,.cta-card')];
  if ('IntersectionObserver' in window) {
    const observer = new IntersectionObserver(entries => {
      entries.forEach(entry => {
        if (entry.isIntersecting) { entry.target.classList.add('is-visible'); observer.unobserve(entry.target); }
      });
    }, { threshold: .08, rootMargin: '0px 0px -24px 0px' });
    reveals.forEach((item, index) => {
      item.classList.add('reveal');
      item.style.setProperty('--reveal-delay', `${Math.min(index % 2 * 80, 80)}ms`);
      observer.observe(item);
    });
    document.documentElement.classList.add('motion-ready');
    const active = new IntersectionObserver(entries => {
      entries.forEach(entry => {
        if (entry.isIntersecting) menu.querySelectorAll('a[href^="#"]').forEach(link => {
          if (link.hash === '#' + entry.target.id) link.setAttribute('aria-current', 'true');
          else link.removeAttribute('aria-current');
        });
      });
    }, { rootMargin: '-15% 0px -60% 0px', threshold: 0 });
    document.querySelectorAll('.section-block[id]').forEach(section => active.observe(section));
  }
  const portrait = document.querySelector('.silicon-portrait');
  let frame = 0;
  function render() {
    frame = 0;
    const range = document.documentElement.scrollHeight - innerHeight;
    const progress = range > 0 ? Math.max(0, Math.min(1, scrollY / range)) : 0;
    nav.style.setProperty('--read-progress', progress.toFixed(4));
    nav.classList.toggle('scrolled', scrollY > 12);
    if (portrait && !reduced.matches) {
      const box = portrait.getBoundingClientRect();
      if (box.bottom > 0 && box.top < innerHeight) {
        const fraction = Math.max(-1, Math.min(1, (box.top + box.height / 2 - innerHeight / 2) / innerHeight));
        portrait.style.setProperty('--photo-y', `${(fraction * 13).toFixed(2)}px`);
        portrait.style.setProperty('--photo-scale', (1.045 + Math.abs(fraction) * .025).toFixed(4));
      }
    }
  }
  function requestRender() { if (!frame && !document.hidden) frame = requestAnimationFrame(render); }
  addEventListener('scroll', requestRender, { passive: true });
  addEventListener('resize', requestRender, { passive: true });
  reduced.addEventListener('change', requestRender);
  document.addEventListener('visibilitychange', () => {
    if (document.hidden && frame) { cancelAnimationFrame(frame); frame = 0; } else requestRender();
  });
  requestRender();
})();

// Both translations use the deployed English heading IDs, preserving deep links.
(() => {
  const link = document.querySelector('.language-switch');
  if (!link) return;
  const target = new URL(link.href);
  const update = () => { target.hash = location.hash; link.href = target.href; };
  update(); addEventListener('hashchange', update);
})();
