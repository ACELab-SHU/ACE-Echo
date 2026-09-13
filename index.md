---
layout: echo-classic
title: ACE-Echo · Communication–AI Convergence
#tags: [getting_started, troubleshooting]
keywords: Echo, Venus, AURA, Feature, Email, Community
summary: "An open development platform for Venus, our programmable communication–AI processor. Explore our architecture, first-generation silicon and the ACE-Echo roadmap."
sidebar: mydoc_sidebar
permalink: index.html
folder: mydoc
hide_sidebar: true
hide_summary: true
hide_title: true
toc: false
---

<main id="main" class="echo-home">
  <div class="hero-grid">
    <div class="hero-copy">
      <p class="eyebrow">Open Platform for Communication-AI Convergence Fusion</p>
      <p class="hero-product">ACE-Echo</p><h1>Communication.<br><span>Intelligence.</span><br>One architecture.</h1>
      <p class="hero-summary">An open development platform built on <strong>Venus</strong>, our programmable RISC-V processor for communication–AI convergence. Explore the architecture. Build applications for our hardware. Shape what comes next.</p>
      <div class="hero-actions">
        <a class="hero-button primary" href="venus1_get_started.html">Explore ACE-Echo 1.0</a>
        <a class="hero-button secondary" href="news.html">Latest Updates</a>
      </div>
    </div>
    <div class="hero-panel">
      <p class="panel-kicker">The Echo ecosystem</p>
      <ul class="hero-list">
        <li>Communication &amp; AI operators for FFT, Decoder, Conv2D/3D, GELU/SiLU and more</li>
        <li>An integrated development workflow with external compiler tools</li>
        <li>Gem5 simulation for Venus application development and approximate performance evaluation</li>
        <li>Real-world application demos: 5G/LTE, AI-Based Channel Estimation, GNSS, LoRa, and more</li>
        <li>An initial Agent-assisted development framework in 1.0</li>
      </ul>
    </div>
  </div>

  <section class="silicon-stage" aria-labelledby="silicon-title">
    <div class="silicon-heading"><p class="section-tag">First-generation silicon</p><h2 id="silicon-title">An idea.<br>Now in your hands.</h2><p>Our first-generation chip has taped out<br>and achieved first light.</p><a href="venus1-first-light.html">Meet the first Venus chip ↗</a></div>
    <figure class="silicon-portrait"><div class="photo-window"><img src="images/venus1-silicon.jpeg" width="1279" height="1706" alt="Our first-generation DT-VENUS-A1 chip after fabrication and packaging"></div><figcaption><span>DT-VENUS-A1</span><span>Real silicon. Successful bring-up.</span></figcaption></figure>
  </section>
  <section class="section-block" id="architecture">
    <div class="section-heading">
      <p class="section-tag">Architecture</p>
      <h2>AURA inside.<br>Venus at the heart.</h2><p class="section-intro">The architecture, the chip and the tools.<br>Designed to evolve together.</p>
    </div>
    <div class="stack-grid">
      <article class="stack-card">
        <h3 class="stack-title">AURA: AI Unified Radio Architecture</h3>
        <p>AURA is the foundational computing architecture behind Echo. It is designed for <strong>tight integration of perception, communication, and computation</strong>, optimized for edge and low-latency scenarios. It is composed of Venus Language, Zoozve Compiler, and Mathematic Libraries.</p>
        <p>If you want to learn more about compilers, see <a href="https://doi.org/10.1145/3735452.3735526">Zoozve: A Strip-Mining-Free RISC-V Vector Extension with Arbitrary Register Grouping Compilation Support (WIP)</a>.</p>
        <div class="visual-frame">
          <img src="images/aura-architecture.png" width="1894" height="830" loading="lazy" alt="AURA programming architecture: Venus Language, Zoozve Compiler and Mathematic Libraries">
        </div>
      </article>
      <article class="stack-card">
        <h3 class="stack-title">Venus: Our RISC-V Communication-AI Chip</h3>
        <p>Venus is a custom RISC-V processor based on the AURA architecture.</p>
        <ul class="feature-list">
          <li>Instruction set extensions for communication and neural workloads</li>
          <li>Built-in accelerators such as the vector engine</li>
          <li>Composed of Venus Tile, Venus Scheduler, and Venus DFE</li>
          <li>Research directions spanning wireless baseband, edge AI and communication–AI integration</li>
        </ul>
        <p>For details, see <a href="https://doi.org/10.1145/3658617.3697558">A Hierarchical Dataflow-Driven Heterogeneous Architecture for Wireless Baseband Processing</a>.</p>
        <div class="visual-frame">
          <img src="images/venus-architecture.png" width="1891" height="831" loading="lazy" alt="Venus RISC-V SoC: Venus Tile, Venus Scheduler and Venus DFE">
        </div>
      </article>
    </div>
  </section>

  <section class="section-block" id="capabilities">
    <div class="section-heading">
      <p class="section-tag">Capabilities</p>
      <h2>Your ideas.<br>Running on Venus.</h2>
    </div>
    <div class="capability-table">
      <div class="release-intro"><span class="version-pill">ACE-Echo 1.0</span><p>A usable Gem5-based simulator and the first version of our Agent-assisted development framework.</p></div>
      <table><thead><tr><th>Capability</th><th>What you can do in 1.0</th></tr></thead><tbody>
        <tr><td>Develop for Venus</td><td>Write application tasks in Venus C and describe their dependencies in BAS.</td></tr>
        <tr><td>Explore performance</td><td>Run the Gem5 model and quickly estimate task timing and data movement.</td></tr>
        <tr><td>Iterate with an Agent</td><td>Use the initial Agent framework with developer guidance and review.</td></tr>
        <tr><td>Build on shared work</td><td>Start with the paired DSL and Venus1 samples, then contribute your own applications.</td></tr>
      </tbody></table>
      <p class="capability-note">Cycle-level simulation supports approximate performance evaluation. See the <a href="venus1_validation.html">validation scope</a> for measured accuracy and model boundaries.</p>
    </div>
  </section>

  <section class="section-block applications" id="applications"><div class="section-heading"><p class="section-tag">Communication meets intelligence</p><h2>One foundation.<br>Many possibilities.</h2><p class="section-intro">Explore the applications and research<br>behind the Echo ecosystem.</p></div><div class="application-grid"><a href="breakthroughs_in_application.html"><span class="application-index">01</span><h3>5G &amp; LTE</h3><p>Synchronization, cell search and baseband processing.</p><span class="round-link" aria-hidden="true">↗</span></a><a href="academic-4.html"><span class="application-index">02</span><h3>Communication + AI</h3><p>Architecture research connecting neural workloads and wireless processing.</p><span class="round-link" aria-hidden="true">↗</span></a><a href="mydoc_supported_features.html"><span class="application-index">03</span><h3>Beyond one protocol</h3><p>Explore GNSS, LoRa and other signal-processing directions.</p><span class="round-link" aria-hidden="true">↗</span></a></div><p class="ecosystem-note">Ecosystem examples and research directions. Available applications and validation coverage depend on the selected hardware and release.</p></section>
  <section class="section-block" id="audience">
    <div class="section-heading">
      <p class="section-tag">Audience</p>
      <h2>A platform for<br>what comes next.</h2>
    </div>
    <div class="audience-grid">
      <article class="audience-card">
        <p class="audience-title">Academia &amp; Researchers</p>
        <p class="audience-subtitle">Open-Source Platform for Communication-AI Research</p>
        <p>Explore communication–AI algorithms and hardware-aware implementations in a shared, programmable research environment.</p>
      </article>
      <article class="audience-card">
        <p class="audience-title">Industry</p>
        <p class="audience-subtitle">Decoupled Software-Hardware Baseband Chip Solution</p>
        <p>Explore applications and system trade-offs for a programmable communication processor.</p>
        <p>Use simulation to compare design choices before hardware evaluation.</p>
      </article>
      <article class="audience-card">
        <p class="audience-title">Standards Organizations</p>
        <p class="audience-subtitle">Explore Future Wireless Systems</p>
        <p>Investigate new wireless processing ideas on an architecture designed around communication and intelligent computation.</p>
        <p>Develop reusable experiments and share findings with the community.</p>
      </article>
    </div>
    <blockquote class="home-quote">
      <strong>Echo is your playground.</strong> Whether you're building, testing, or scaling, Echo gives you the freedom to create.
    </blockquote>
  </section>

  <section class="section-block" id="roadmap">
    <div class="section-heading">
      <p class="section-tag">Roadmap</p>
      <h2>Assistance today.<br>Intent-driven development next.</h2>
    </div>
    {% include echo-roadmap.html %}
  </section>

  <section class="section-block latest-news" id="news"><div class="section-heading"><p class="section-tag">Latest from ACE Lab</p><h2>Silicon is only<br>the beginning.</h2></div><a class="news-feature" href="venus1-first-light.html"><span class="news-kicker">CHIP MILESTONE · 2026</span><h3>First-generation chip.<br>Successful tape-out. First light.</h3><p>From fabrication and packaging to bring-up and real-signal NR cell search.</p><span class="news-more">Read the announcement ↗</span></a><a class="all-news" href="news.html">All project news ↗</a></section>
  <section class="section-block cta-section" id="get-started">
    <div class="cta-card">
      <div>
        <p class="section-tag">Get Started</p>
        <h2>Start building with Echo now</h2>
      </div>
      <ol class="start-list">
        <li>Visit our quick-start guide: <a href="venus1_get_started.html">Get started with ACE-Echo 1.0</a></li>
        <li>Get the platform source and configure the external compiler tools.</li>
        <li>Join the community and start building.</li>
      </ol>
    </div>
    <div class="cta-card community-card">
      <p class="section-tag">Community</p>
      <h2>Join the Echo Community</h2>
      <p>Email: <a href="mailto:shenyihao@shu.edu.cn">shenyihao@shu.edu.cn</a></p>
      <p><a href="https://github.com/ACELab-SHU/ACE-Echo">Explore the source on GitHub ↗</a></p>
      <blockquote class="closing-quote">A shared foundation for communication and intelligence.</blockquote>
      <p>We warmly welcome more developers, researchers, and collaborators to join us on this journey.</p>
    </div>
  </section>
</main>
