(() => {
  'use strict';
  const input = document.getElementById('search-input');
  const results = document.getElementById('results-container');
  if (!input || !results) return;
  const isZh = document.documentElement.lang === 'zh-CN';
  let entries = [], ready = false, failed = false;
  function notice(text) { const li = document.createElement('li'); li.textContent = text; results.append(li); }
  function render() {
    results.replaceChildren();
    const query = input.value.trim().toLocaleLowerCase();
    if (!query) return;
    if (failed) { notice(isZh ? '搜索暂时不可用，请使用下方文档导航。' : 'Search is unavailable. Use the documentation navigation below.'); return; }
    if (!ready) { notice(isZh ? '正在加载搜索…' : 'Loading search…'); return; }
    const terms = query.split(/\s+/);
    const found = entries.filter(item => terms.every(term => item.text.includes(term)))
      .sort((a, b) => Number(b.title.includes(query)) - Number(a.title.includes(query)))
      .slice(0, 8);
    if (!found.length) { notice(isZh ? '未找到相关内容。' : 'No results found.'); return; }
    for (const item of found) {
      const li = document.createElement('li'), link = document.createElement('a');
      link.href = item.record.url; link.textContent = item.record.title;
      li.append(link); results.append(li);
    }
  }
  input.addEventListener('input', event => { if (!event.isComposing) render(); });
  input.addEventListener('compositionend', render);
  fetch(isZh ? 'search-zh.json' : 'search.json')
    .then(response => { if (!response.ok) throw new Error('Search index unavailable'); return response.json(); })
    .then(data => {
      entries = data.filter(record => /^[a-zA-Z0-9_-]+\.html$/.test(record.url)).map(record => ({
        record, title: String(record.title).toLocaleLowerCase(),
        text: [record.title, record.keywords, record.summary, record.body].join(' ').toLocaleLowerCase()
      }));
      ready = true; render();
    }).catch(() => { failed = true; render(); });
})();
