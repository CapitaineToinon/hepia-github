<script setup lang="ts">
import Toast from "./Toast.vue";
import { useToastMap } from "../composable/useToast";

const toasts = useToastMap();
</script>

<template>
  <div class="absolute top-0 right-0 overflow-hidden z-50 pointer-events-none">
    <div class="p-6 flex flex-col gap-3">
      <transition-group name="slide">
        <Toast v-for="toast in toasts" :key="toast.id" :toast="toast" />
      </transition-group>
    </div>
  </div>
</template>

<style lang="postcss" scoped>
.slide-leave-active,
.slide-enter-active {
  @apply transition-all;
}
.slide-leave-to {
  @apply transform translate-x-full opacity-0;
}
</style>
