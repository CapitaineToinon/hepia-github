<script setup lang="ts">
import {
  useDark,
  useToggle,
  set,
  get,
  useEventListener,
  useThrottleFn,
} from "@vueuse/core";
import { computed, ref } from "vue";
import { useAuthStateStore, useAuthStore } from "../stores/auth";
import { HomeIcon, MoonIcon, SunIcon, MenuIcon } from "@heroicons/vue/outline";
import { useRouter } from "vue-router";

const router = useRouter();

const isDark = useDark({
  selector: "html",
  attribute: "data-theme",
  valueDark: "dark",
  valueLight: "light",
});

const menu = ref(false);
const toggle = useToggle(isDark);

const store = useAuthStateStore();
const authStore = useAuthStore();

const buttonText = computed(() =>
  store.authState.isAuthenticated ? "Sign out" : "Sign in"
);

function onClick() {
  store.authState.isAuthenticated
    ? authStore.$auth.signOut()
    : authStore.$auth.signInWithRedirect();
}

function openMenu() {
  set(menu, !get(menu));
}

function closeMenu() {
  set(menu, false);
}

router.beforeEach(closeMenu);

useEventListener("resize", useThrottleFn(closeMenu, 100));
</script>

<template>
  <div class="navbar mb-3 shadow-lg bg-base-100 text-base-content">
    <div class="px-2 mx-2 navbar-start">
      <RouterLink to="/" class="btn btn-square btn-ghost">
        <HomeIcon class="h-6 w-6" />
      </RouterLink>
    </div>
    <div
      class="mobile-menu px-2 mx-2 navbar-center sm:flex"
      :class="{ hidden: !menu }"
    >
      <div class="flex items-stretch">
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
    </div>
    <div class="navbar-end">
      <button @click="onClick" class="btn btn-ghost btn-sm rounded-btn">
        {{ buttonText }}
      </button>
      <button class="btn btn-square btn-ghost text-2xl" @click="() => toggle()">
        <MoonIcon class="h-6 w-6" :class="{ hidden: isDark }" />
        <SunIcon class="h-6 w-6" :class="{ hidden: !isDark }" />
      </button>
      <button
        class="sm:hidden btn btn-square btn-ghost text-2xl"
        @click="() => openMenu()"
      >
        <MenuIcon class="h-6 w-6" />
      </button>
    </div>
  </div>
</template>

<style lang="postcss" scoped>
.navbar {
  @apply relative z-40;

  & .mobile-menu {
    @apply absolute right-0 left-0 top-full;
    @apply bg-base-100 shadow-lg m-0 py-3 justify-center;
    @apply sm:relative sm:shadow-none sm:bg-transparent;
  }
}

.router-link-active {
  @apply text-accent;
}
</style>
