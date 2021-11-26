import { createRouter, createWebHistory } from "vue-router";
import { LoginCallback } from "@okta/okta-vue";
import Home from "./pages/index.vue";
import Students from "./pages/Students.vue";
import Teachers from "./pages/Teachers.vue";

export const router = createRouter({
  history: createWebHistory(),
  routes: [
    {
      path: "/",
      name: 'home',
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
      path: "/login/callback",
      component: LoginCallback,
    },
  ],
});
