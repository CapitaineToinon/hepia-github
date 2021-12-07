import type { Toast, ToastType } from "../types";
import type { Ref } from "vue";
import { useInterval, invoke, until, get } from "@vueuse/core";
import { computed, reactive, watchEffect, ref, watch } from "vue";

const state = reactive({
  map: new Map<number, Ref<Toast>>(),
  index: 0,
});

export function useToast() {
  function add(message: string, type: ToastType) {
    const id = state.index++;
    const ttl = 5000;

    const { counter, pause, resume } = useInterval(16, {
      controls: true,
      immediate: false,
    });

    const toast: Ref<Toast> = ref<Toast>({
      id,
      message,
      type,
      progress: 0,
      remove: () => state.map.delete(id),
      pause: () => pause(),
      resume: () => {
        if (state.map.has(id)) {
          resume();
        }
      },
    });

    watch(counter, (v) => {
      toast.value.progress = (v * 16 * 100) / ttl;
    });

    invoke(async () => {
      resume();
      await until(counter).toMatch((v) => v > ttl / 16);
      pause();
      toast.value.remove();
    });

    state.map.set(id, toast);
  }

  return {
    add: (message: string) => add(message, "primary"),
    success: (message: string) => add(message, "success"),
    error: (message: string) => add(message, "error"),
  };
}

export function useToastMap() {
  return computed(() =>
    Array.from(state.map.values()).map((toast) => get(toast))
  );
}

watchEffect(() => console.log(state));
