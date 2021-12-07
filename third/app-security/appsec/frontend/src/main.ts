import "./index.css";
import App from "./App.vue";
import { createApp } from "vue";
import { router } from "./router";
import { OktaAuth } from "@okta/okta-auth-js";
import OktaVue from "@okta/okta-vue";
import { createPinia } from "pinia";

const oktaAuth = new OktaAuth({
  issuer: import.meta.env.VITE_OKTA_ISSUER as string,
  clientId: import.meta.env.VITE_OKTA_CLIENT_ID as string,
  redirectUri: window.location.origin + "/login/callback",
  scopes: ["openid", "profile", "email"],
});

createApp(App)
  .use(createPinia())
  .use(OktaVue, { oktaAuth })
  .use(router)
  .mount("#app");
