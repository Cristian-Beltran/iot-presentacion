import { defineConfig } from 'vite'

// Vite 8 + Lightning CSS currently misparses one generated UnoCSS selector
// during minification. The CSS is valid before that optimization, so the deck
// disables only CSS minification while keeping the rest of the production build.
export default defineConfig({
  build: {
    cssMinify: false,
  },
})
