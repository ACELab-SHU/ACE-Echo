---
layout: echo-classic
title: ACE-Echo · 通信与 AI 融合
keywords: ACE-Echo, Venus, ACE-Echo · 通信与 AI 融合
summary: 面向 Venus 可编程通信–AI 处理器的开源开发平台。了解架构、第一代芯片与 ACE-Echo 路线图。
sidebar: mydoc_sidebar_zh
permalink: zh_index.html
folder: mydoc
hide_sidebar: true
hide_summary: true
hide_title: true
toc: false
lang: zh-CN
translation_url: index.html
content_status: ''
---

<main id="main" class="echo-home">
  <div class="hero-grid"><div class="hero-copy">
    <p class="eyebrow">通信与 AI 融合芯片的开源开发平台</p>
    <p class="hero-product">ACE-Echo</p><h1 class="hero-title-zh"><span class="hero-line">通信</span><span class="hero-line hero-accent">智能</span><span class="hero-line">同一架构</span></h1>
    <p class="hero-summary">以可编程 RISC-V 通信–AI 融合处理器 <strong>Venus</strong> 为基础。了解我们的架构，为真实硬件开发应用，一起探索下一步。</p>
    <div class="hero-actions"><a class="hero-button primary" href="zh_venus1_get_started.html">探索 ACE-Echo 1.0</a><a class="hero-button secondary" href="zh_news.html">项目动态</a></div>
  </div><div class="hero-panel"><p class="panel-kicker">Echo 生态</p><ul class="hero-list">
    <li>围绕 FFT、译码、Conv2D/3D、GELU/SiLU 等通信与 AI 算子开展研究</li>
    <li>连接外部编译工具的集成开发工作流</li>
    <li>用 Gem5 开发 Venus 应用，快速评估大致性能</li>
    <li>涵盖 5G/LTE、AI 信道估计，以及 GNSS、LoRa 等应用探索方向</li>
    <li>1.0 提供初版 Agent 辅助开发框架</li>
  </ul></div></div>

  <section class="silicon-stage" aria-labelledby="silicon-title">
    <div class="silicon-heading"><p class="section-tag">第一代芯片</p><h2 id="silicon-title">从构想到实物。<br>已经点亮。</h2><p>第一代芯片已完成流片，<br>并成功在真实硅片上运行。</p><a href="zh_venus1-first-light.html">认识第一颗 Venus 芯片 ↗</a></div>
    <figure class="silicon-portrait"><div class="photo-window"><img src="images/venus1-silicon.jpeg" width="1279" height="1706" alt="完成制造与封装的第一代 DT-VENUS-A1 芯片"></div><figcaption><span>DT-VENUS-A1</span><span>真实芯片，成功点亮。</span></figcaption></figure>
  </section>
  <section class="section-block" id="architecture"><div class="section-heading"><p class="section-tag">架构</p><h2><span class="display-line">AURA 为基础</span><span class="display-line">Venus 为核心</span></h2><p class="section-intro"><span class="display-line">架构、芯片与工具</span><span class="display-line">共同演进</span></p></div>
    <div class="stack-grid"><article class="stack-card"><h3 class="stack-title">AURA：AI Unified Radio Architecture</h3><p>AURA 是 Echo 背后的计算架构基础，面向边缘与低时延场景，探索<strong>感知、通信与计算的紧密融合</strong>。其组成包括 Venus Language、Zoozve 编译器和数学库。</p><p>编译器研究见 <a href="https://doi.org/10.1145/3735452.3735526">Zoozve：支持任意寄存器分组编译、无需 strip-mining 的 RISC-V 向量扩展（WIP）</a>。</p><div class="visual-frame"><img src="images/aura-architecture.png" width="1894" height="830" loading="lazy" alt="AURA 编程架构：Venus Language、Zoozve 编译器与数学库"></div></article>
    <article class="stack-card"><h3 class="stack-title">Venus：RISC-V 通信–AI 融合芯片</h3><p>Venus 是基于 AURA 架构的定制 RISC-V 处理器。</p><ul class="feature-list"><li>面向通信与神经网络工作负载的指令扩展</li><li>集成向量引擎等加速能力</li><li>由 Venus Tile、Venus Scheduler 和 Venus DFE 组成</li><li>探索无线基带、边缘 AI 与通信–AI 融合</li></ul><p>详细设计见<a href="https://doi.org/10.1145/3658617.3697558">面向无线基带处理的分层数据流驱动异构架构</a>。</p><div class="visual-frame"><img src="images/venus-architecture.png" width="1891" height="831" loading="lazy" alt="Venus RISC-V SoC：Tile、Scheduler 与数字前端 DFE"></div></article></div>
  </section>
  <section class="section-block" id="capabilities"><div class="section-heading"><p class="section-tag">平台能力</p><h2><span class="display-line">让你的想法</span><span class="display-line">运行在 Venus 上</span></h2></div><div class="capability-table"><div class="release-intro"><span class="version-pill">ACE-Echo 1.0</span><p>好用的 Gem5 仿真器，与初版 Agent 辅助开发框架。</p></div><table><thead><tr><th>能力</th><th>在 1.0 中可以做什么</th></tr></thead><tbody><tr><td>面向 Venus 开发</td><td>用 Venus C 编写应用任务，用 BAS 描述依赖。</td></tr><tr><td>探索性能</td><td>运行 Gem5 模型，快速估计任务时序和数据搬运。</td></tr><tr><td>与 Agent 共同迭代</td><td>在开发者指导与审核下使用初版 Agent 框架。</td></tr><tr><td>从已有成果出发</td><td>使用配套 DSL 与 Venus1 示例，贡献自己的应用。</td></tr></tbody></table><p class="capability-note">周期级仿真用于近似性能评估。测量精度与模型边界见<a href="zh_venus1_validation.html">验证范围</a>。</p></div></section>
  <section class="section-block applications" id="applications"><div class="section-heading"><p class="section-tag">通信与智能相遇</p><h2><span class="display-line">同一基础</span><span class="display-line">更多可能</span></h2><p class="section-intro"><span class="display-line">探索 Echo 生态中的</span><span class="display-line">应用与研究</span></p></div><div class="application-grid"><a href="zh_breakthroughs_in_application.html"><span class="application-index">01</span><h3>5G 与 LTE</h3><p>同步、小区搜索与基带处理。</p><span class="round-link" aria-hidden="true">↗</span></a><a href="zh_academic-4.html"><span class="application-index">02</span><h3>通信 + AI</h3><p>连接神经网络工作负载与无线处理的架构研究。</p><span class="round-link" aria-hidden="true">↗</span></a><a href="zh_mydoc_supported_features.html"><span class="application-index">03</span><h3>探索更多协议</h3><p>GNSS、LoRa 与更多信号处理方向。</p><span class="round-link" aria-hidden="true">↗</span></a></div><p class="ecosystem-note">这里展示生态示例与研究方向。可用应用及验证覆盖取决于所选硬件与发布版本。</p></section>
  <section class="section-block" id="audience"><div class="section-heading"><p class="section-tag">面向开发者与研究者</p><h2><span class="display-line">为下一步创新</span><span class="display-line">提供共同平台</span></h2></div><div class="audience-grid">
    <article class="audience-card"><p class="audience-title">学术界与研究者</p><p class="audience-subtitle">通信–AI 研究的开源平台</p><p>在共享、可编程的研究环境中，探索通信–AI 算法及面向硬件的实现。</p></article>
    <article class="audience-card"><p class="audience-title">产业界</p><p class="audience-subtitle">软硬件解耦的基带芯片方案</p><p>探索可编程通信处理器的应用与系统权衡。</p><p>在硬件评估前，通过仿真比较不同设计选择。</p></article>
    <article class="audience-card"><p class="audience-title">标准组织</p><p class="audience-subtitle">探索未来无线系统</p><p>在围绕通信与智能计算设计的架构上，研究新的无线处理思路。</p><p>构建可复用实验，与社区分享发现。</p></article></div><blockquote class="home-quote"><strong>在 Echo 上自由探索。</strong> 从构建、测试到扩展，让想法逐步成为应用。</blockquote></section>
  <section class="section-block" id="roadmap"><div class="section-heading"><p class="section-tag">路线图</p><h2><span class="display-line">今天，辅助开发</span><span class="display-line">下一步，意图驱动</span></h2></div>{% include echo-roadmap.html %}</section>
  <section class="section-block latest-news" id="news"><div class="section-heading"><p class="section-tag">ACE Lab 最新动态</p><h2><span class="display-line">芯片点亮</span><span class="display-line">只是开始</span></h2></div><a class="news-feature" href="zh_venus1-first-light.html"><span class="news-kicker">芯片里程碑 · 2026</span><h3>第一代芯片。<br>流片成功，顺利点亮。</h3><p>从制造、封装到上电调试，再到真实信号 NR 小区搜索。</p><span class="news-more">阅读新闻 ↗</span></a><a class="all-news" href="zh_news.html">全部项目新闻 ↗</a></section>
  <section class="section-block cta-section" id="get-started"><div class="cta-card"><div><p class="section-tag">开始使用</p><h2>现在，用 Echo 开发</h2></div><ol class="start-list"><li>阅读<a href="zh_venus1_get_started.html">ACE-Echo 1.0 入门指南</a>。</li><li>获取平台源码，配置外部编译工具。</li><li>加入社区，开始构建应用。</li></ol></div><div class="cta-card community-card"><p class="section-tag">社区</p><h2>加入 Echo 社区</h2><p>邮箱：<a href="mailto:shenyihao@shu.edu.cn">shenyihao@shu.edu.cn</a></p><p><a href="https://github.com/ACELab-SHU/ACE-Echo">在 GitHub 探索源码 ↗</a></p><blockquote class="closing-quote">通信与智能，共享同一基础。</blockquote><p>欢迎更多开发者、研究者与合作伙伴共同参与。</p></div></section>
</main>
