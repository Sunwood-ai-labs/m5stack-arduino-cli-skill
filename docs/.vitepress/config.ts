import { defineConfig } from "vitepress";

const repoName = "m5stack-arduino-cli-skill";

export default defineConfig({
  title: "M5Stack Arduino CLI Skill",
  description:
    "Codex skill docs for setting up and troubleshooting M5Stack boards with Arduino CLI on Windows.",
  base: `/${repoName}/`,
  cleanUrls: true,
  lastUpdated: true,
  ignoreDeadLinks: false,
  head: [["link", { rel: "icon", href: `/${repoName}/icon.svg` }]],
  themeConfig: {
    logo: "/icon.svg",
    search: {
      provider: "local"
    },
    socialLinks: [
      {
        icon: "github",
        link: "https://github.com/Sunwood-ai-labs/m5stack-arduino-cli-skill"
      }
    ]
  },
  locales: {
    root: {
      label: "English",
      lang: "en",
      link: "/",
      themeConfig: {
        nav: [
          { text: "Guide", link: "/guide/quickstart" },
          { text: "Diagnosis", link: "/guide/diagnosis" },
          { text: "GitHub", link: "https://github.com/Sunwood-ai-labs/m5stack-arduino-cli-skill" }
        ],
        sidebar: [
          {
            text: "Guide",
            items: [
              { text: "Quick Start", link: "/guide/quickstart" },
              { text: "Diagnosis Playbook", link: "/guide/diagnosis" }
            ]
          }
        ],
        outline: {
          level: [2, 3],
          label: "On this page"
        },
        docFooter: {
          prev: "Previous page",
          next: "Next page"
        },
        darkModeSwitchLabel: "Appearance",
        sidebarMenuLabel: "Menu",
        returnToTopLabel: "Back to top",
        langMenuLabel: "Languages",
        lightModeSwitchTitle: "Switch to light theme",
        darkModeSwitchTitle: "Switch to dark theme"
      }
    },
    ja: {
      label: "日本語",
      lang: "ja",
      link: "/ja/",
      themeConfig: {
        nav: [
          { text: "ガイド", link: "/ja/guide/quickstart" },
          { text: "診断", link: "/ja/guide/diagnosis" },
          { text: "GitHub", link: "https://github.com/Sunwood-ai-labs/m5stack-arduino-cli-skill" }
        ],
        sidebar: [
          {
            text: "ガイド",
            items: [
              { text: "クイックスタート", link: "/ja/guide/quickstart" },
              { text: "診断プレイブック", link: "/ja/guide/diagnosis" }
            ]
          }
        ],
        outline: {
          level: [2, 3],
          label: "このページ"
        },
        docFooter: {
          prev: "前のページ",
          next: "次のページ"
        },
        darkModeSwitchLabel: "テーマ",
        sidebarMenuLabel: "メニュー",
        returnToTopLabel: "トップへ戻る",
        langMenuLabel: "言語",
        lightModeSwitchTitle: "ライトテーマへ切り替え",
        darkModeSwitchTitle: "ダークテーマへ切り替え"
      }
    }
  }
});
