(() => {
  const navigation = document.querySelector('.doc-navigation');
  const narrow = matchMedia('(max-width: 900px)');
  function updateNavigation() { if (navigation) navigation.open = !narrow.matches; }
  updateNavigation(); narrow.addEventListener('change', updateNavigation);
  document.querySelectorAll('.doc-main table').forEach(table => {
    if (table.closest('.table-scroll,.dataTables_wrapper')) return;
    const wrap = document.createElement('div');
    wrap.className = 'table-scroll'; wrap.tabIndex = 0;
    wrap.setAttribute('role', 'region'); wrap.setAttribute('aria-label', 'Scrollable table');
    table.before(wrap); wrap.append(table);
  });
  document.querySelectorAll('.doc-sidebar a').forEach(link => {
    if (new URL(link.href).pathname === location.pathname) link.setAttribute('aria-current', 'page');
  });
})();
