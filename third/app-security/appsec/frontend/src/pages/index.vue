<script setup lang="ts">
import { useAuthStateStore, useUserClaimsStore } from "../stores/auth";

const claimsStore = useUserClaimsStore();
const stateStore = useAuthStateStore();
</script>

<template>
  <div class="hero min-h-screen bg-base-100">
    <div class="text-center hero-content">
      <div v-if="claimsStore.userClaims" class="max-w-md">
        <h1 class="mb-5 text-5xl font-bold">
          Hello {{ claimsStore.userClaims.name }}
        </h1>
        <p class="mb-5">
          Provident cupiditate voluptatem et in. Quaerat fugiat ut assumenda
          excepturi exercitationem quasi. In deleniti eaque aut repudiandae et a
          id nisi.
        </p>
        <button class="btn btn-primary">Get Started</button>
      </div>
      <div
        v-if="stateStore.authState.isAuthenticated && !claimsStore.userClaims"
      >
        Loading user...
      </div>
    </div>
  </div>
</template>
