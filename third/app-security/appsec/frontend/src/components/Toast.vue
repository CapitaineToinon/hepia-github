<script setup lang="ts">
import type { Toast } from "../types";
import { reactive } from "vue";
import { controlledComputed } from "@vueuse/shared";

const props = defineProps<{
  toast: Toast;
}>();

const classes = reactive({
  primary: props.toast.type === "primary",
  success: props.toast.type === "success",
  error: props.toast.type === "error",
});

const progress = controlledComputed(
  () => props.toast.progress,
  () => props.toast.progress
);
</script>

<template>
  <div
    @mouseenter="toast.pause"
    @mouseleave="toast.resume"
    class="toast w-96 card shadow-lg pointer-events-auto"
    :class="classes"
  >
    <div
      class="
        card-body
        py-3
        pl-6
        pr-3
        flex flex-row
        items-center
        justify-between
        gap-3
      "
    >
      {{ toast.message }}
      <button @click="toast.remove" class="btn btn-ghost btn-square">
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
            d="M6 18L18 6M6 6l12 12"
          />
        </svg>
      </button>
    </div>
    <progress class="progress opacity-50" :value="progress" max="100" />
  </div>
</template>

<style lang="postcss" scoped>
.toast {
  @apply relative;
  &.success,
  &.primary {
    @apply bg-primary text-primary-content;
  }

  &.error {
    @apply bg-error text-neutral-content;
  }

  & .progress {
    @apply absolute right-0 bottom-0 left-0;
  }
}
</style>
