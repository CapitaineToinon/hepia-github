<script setup lang="ts">
import { useDark, useToggle } from "@vueuse/core";
import { computed, watchEffect } from "vue";
import { useAuthStateStore, useAuthStore } from "../stores/auth";

const isDark = useDark({
  selector: "html",
  attribute: "data-theme",
  valueDark: "dark",
  valueLight: "light",
});

const toggle = useToggle(isDark);

const store = useAuthStateStore();
const authStore = useAuthStore();

const buttonText = computed(() =>
  store.authState.isAuthenticated ? "Sign out" : "Sign in"
);

watchEffect(() => console.log(store.authState.isAuthenticated));

function onClick() {
  store.authState.isAuthenticated
    ? authStore.$auth.signOut()
    : authStore.$auth.signInWithRedirect();
}
</script>

<template>
  <div class="navbar" :class="{ 'is-fixed': $route.name === 'home' }">
    <div class="flex-1 flex gap-3">
      <RouterLink to="/" class="btn btn-square btn-ghost">
        <svg
          xmlns="http://www.w3.org/2000/svg"
          class="h-6 w-6"
          fill="none"
          viewBox="0 0 24 24"
          stroke="currentColor"
        >
          <path
            stroke-linecap="round"
            stroke-linejoin="round"
            stroke-width="2"
            d="M3 12l2-2m0 0l7-7 7 7M5 10v10a1 1 0 001 1h3m10-11l2 2m-2-2v10a1 1 0 01-1 1h-3m-6 0a1 1 0 001-1v-4a1 1 0 011-1h2a1 1 0 011 1v4a1 1 0 001 1m-6 0h6"
          />
        </svg>
      </RouterLink>
      <RouterLink to="/students" class="btn btn-ghost btn-sm rounded-btn">
        Students
      </RouterLink>
      <RouterLink to="/teachers" class="btn btn-ghost btn-sm rounded-btn">
        Teachers
      </RouterLink>
    </div>
    <div class="flex-none flex gap-3">
      <button @click="onClick" class="btn btn-ghost btn-sm rounded-btn">
        {{ buttonText }}
      </button>
      <button class="btn btn-square btn-ghost text-2xl" @click="() => toggle()">
        {{ isDark ? "🌝" : "🌚" }}
      </button>
    </div>
  </div>
</template>

<style lang="postcss" scoped>
.navbar {
  @apply mb-6 text-base-content;
  @apply shadow-md bg-base-300;

  &.is-fixed {
    @apply fixed top-0 left-0 right-0;
  }
}

.router-link-active {
  @apply text-primary;
}
</style>
