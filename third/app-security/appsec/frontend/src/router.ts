import { LoginCallback } from "@okta/okta-vue";
import { createRouter, createWebHistory } from "vue-router";
import Home from "./pages/index.vue";
import Students from "./pages/students.vue";
import Teachers from "./pages/teachers.vue";
import Token from "./pages/token.vue";

export const router = createRouter({
  history: createWebHistory(),
  routes: [
    {
      path: "/",
      name: "home",
      component: Home,
    },
    {
      path: "/students",
      component: Students,
      meta: {
        requiresAuth: true,
      },
    },
    {
      path: "/teachers",
      component: Teachers,
      meta: {
        requiresAuth: true,
      },
    },
    {
      path: "/token",
      component: Token,
      meta: {
        requiresAuth: true,
      },
    },
    {
      path: "/login/callback",
      component: LoginCallback,
    },
  ],
});
