<script setup lang="ts">
import Navbar from "./components/Navbar.vue";
import ToastContainer from "./components/ToastContainer.vue";
</script>

<template>
  <main class="main">
    <ToastContainer />
    <Navbar />
    <div class="container max-w-screen-md mx-auto px-2 sm:px-3">
      <RouterView />
    </div>
  </main>
</template>

<style lang="postcss" scoped>
.main {
  @apply min-h-screen bg-base-200;
}
</style>
