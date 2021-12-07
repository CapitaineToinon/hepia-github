<script setup lang="ts">
import Navbar from "./components/Navbar.vue";
</script>

<template>
  <main class="main">
    <Navbar />
    <RouterView />
  </main>
</template>

<style lang="postcss" scoped>
.main {
  @apply min-h-screen bg-base-200;
}
</style>
