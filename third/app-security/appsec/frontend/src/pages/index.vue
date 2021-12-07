<script setup lang="ts">
import { useAuthStateStore, useUserClaimsStore } from "../stores/auth";

const claimsStore = useUserClaimsStore();
const stateStore = useAuthStateStore();
</script>

<template>
  <div class="hero min-h-screen">
    <div class="text-center hero-content">
      <div v-if="claimsStore.userClaims" class="max-w-md">
        <h1 class="mb-5 text-5xl font-bold">
          Hello, {{ claimsStore.userClaims.name }}
        </h1>
        <p class="mb-5">
          You can now browse the website or you can get your JWT Token to
          comsume the api without using the website.
        </p>
        <RouterLink to="/token" class="btn btn-primary"
          >Get my JWT Token</RouterLink
        >
      </div>
      <div
        v-if="stateStore.authState.isAuthenticated && !claimsStore.userClaims"
      >
        Loading user...
      </div>
    </div>
  </div>
</template>
