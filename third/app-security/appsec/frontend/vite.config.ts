import { defineConfig } from "vite";
import vue from "@vitejs/plugin-vue";
import path from "path"

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [vue()],
  server: {
    port: 8080,
  },
  envDir: path.resolve(__dirname, "../"),
  resolve: {
    alias: [
      {
        // meme workaround because okta sucks major d's
        find: "@okta/okta-auth-js",
        replacement: require.resolve(
          "@okta/okta-auth-js/dist/okta-auth-js.umd.js"
        ),
      },
    ],
  },
});
