import { defineConfig } from "vite";
import vue from "@vitejs/plugin-vue";

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [vue()],
  server: {
    port: 8080,
  },
  resolve: {
    alias: [
      {
        find: "@okta/okta-auth-js",
        replacement: require.resolve(
          "@okta/okta-auth-js/dist/okta-auth-js.umd.js"
        ),
      },
    ],
  },
});
