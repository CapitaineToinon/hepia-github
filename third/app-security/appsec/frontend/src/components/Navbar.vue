<script setup lang="ts">
import { useDark, useToggle } from "@vueuse/core";
import { computed, watchEffect } from "vue";
import { useAuthStateStore, useAuthStore } from "../stores/auth";
import { HomeIcon, MoonIcon, SunIcon } from "@heroicons/vue/outline";

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
        <HomeIcon class="h-6 w-6" />
      </RouterLink>
      <RouterLink to="/students" class="btn btn-ghost btn-sm rounded-btn">
        Students
      </RouterLink>
      <RouterLink to="/teachers" class="btn btn-ghost btn-sm rounded-btn">
        Teachers
      </RouterLink>
      <RouterLink to="/token" class="btn btn-ghost btn-sm rounded-btn">
        Token
      </RouterLink>
    </div>
    <div class="flex-none flex gap-3">
      <button @click="onClick" class="btn btn-ghost btn-sm rounded-btn">
        {{ buttonText }}
      </button>
      <button class="btn btn-square btn-ghost text-2xl" @click="() => toggle()">
        <MoonIcon class="h-6 w-6" :class="{ hidden: isDark }" />
        <SunIcon class="h-6 w-6" :class="{ hidden: !isDark }" />
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
