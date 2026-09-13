(() => {
  const navigation = document.querySelector('.doc-navigation');
  const narrow = matchMedia('(max-width: 900px)');
  function updateNavigation() { if (navigation) navigation.open = !narrow.matches; }
  updateNavigation(); narrow.addEventListener('change', updateNavigation);
  document.querySelectorAll('.doc-main table').forEach(table => {
    if (table.closest('.table-scroll,.dataTables_wrapper')) return;
    const wrap = document.createElement('div');
    wrap.className = 'table-scroll'; wrap.tabIndex = 0;
    wrap.setAttribute('role', 'region'); wrap.setAttribute('aria-label', document.documentElement.lang === 'zh-CN' ? '可横向滚动的表格' : 'Scrollable table');
    table.before(wrap); wrap.append(table);
  });
  const groups = [...document.querySelectorAll('.doc-navigation nav > details.sidebar-group')];
  let activeGroup = null;
  document.querySelectorAll('.doc-sidebar nav a').forEach(link => {
    if (new URL(link.href).pathname !== location.pathname) return;
    link.setAttribute('aria-current', 'page');
    let parent = link.parentElement;
    while (parent && parent !== navigation) {
      if (parent.matches('details')) parent.open = true;
      if (groups.includes(parent)) activeGroup = parent;
      parent = parent.parentElement;
    }
  });
  if (!activeGroup && groups.length) groups[0].open = true;
  groups.forEach(group => group.addEventListener('toggle', () => {
    if (group.open) groups.forEach(other => { if (other !== group) other.open = false; });
  }));
})();
