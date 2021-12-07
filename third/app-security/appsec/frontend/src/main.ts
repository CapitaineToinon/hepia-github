import "./index.css";
import App from "./App.vue";
import { createApp } from "vue";
import { router } from "./router";
import { OktaAuth } from "@okta/okta-auth-js";
import OktaVue from "@okta/okta-vue";
import { createPinia } from "pinia";

const oktaAuth = new OktaAuth({
  issuer: "https://dev-29413258.okta.com/oauth2/default",
  clientId: "0oa2voyb6y84LPhx05d7",
  redirectUri: window.location.origin + "/login/callback",
  scopes: ["openid", "profile", "email"],
});

createApp(App)
  .use(createPinia())
  .use(OktaVue, { oktaAuth })
  .use(router)
  .mount("#app");
