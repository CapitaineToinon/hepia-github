<script setup lang="ts">
import { ref } from "vue";
import { useAuthStore } from "../stores/auth";
import { useClipboard } from "@vueuse/core";
import { useToast } from "../composable/useToast";

const toast = useToast();

const authStore = useAuthStore();
const source = ref(authStore.$auth.getAccessToken() as string);
const { copy, isSupported } = useClipboard({ source });

async function onClick() {
  await copy();
  toast.success("Token saved to clipboard");
}
</script>

<template>
  <div class="prose mb-6">
    <h1>Token</h1>
    <template v-if="authState.isAuthenticated">

    </template>
    <p>
      Here is your JWT token if you wish to use the API through another mean,
      such as
      <a
        class="link link-primary"
        target="__blank"
        href="https://insomnia.rest/"
        >insomnia</a
      >.
    </p>
    <button
      v-if="isSupported"
      @click="onClick"
      class="btn btn-primary flex gap-3 mr-auto mb-3"
    >
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
          d="M8 16H6a2 2 0 01-2-2V6a2 2 0 012-2h8a2 2 0 012 2v2m-6 12h8a2 2 0 002-2v-8a2 2 0 00-2-2h-8a2 2 0 00-2 2v8a2 2 0 002 2z"
        />
      </svg>
      copy to clipboard
    </button>
  </div>
  <div class="card bg-base-100 shadow-lg">
    <div class="card-body">
      <pre class="overflow-x-auto">{{ authStore.$auth.getAccessToken() }}</pre>
    </div>
  </div>
</template>
