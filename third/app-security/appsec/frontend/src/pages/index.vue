<script setup lang="ts">
import { unref, watchEffect } from "vue";
import { useAuthStateStore, useUserClaimsStore } from "../stores/auth";

const claimsStore = useUserClaimsStore();
const stateStore = useAuthStateStore();

watchEffect(() => console.log({ ...unref(claimsStore.userClaims) }));
watchEffect(() => console.log({ ...unref(stateStore.authState.accessToken) }));
</script>

<template>
  <div class="py-6">
    <h1
      class="
        py-5
        text-center
        text-6xl
        font-extrabold
        uppercase
        text-transparent
        bg-clip-text bg-gradient-to-r
        from-primary
        to-accent
      "
    >
      security app
    </h1>

    <div class="prose prose-xl grid grid-cols-1 max-w-screen-lg mx-auto gap-6">
      <div>
        <p>
          You can either use the website to browse
          <RouterLink to="/students" class="link link-primary"
            >students</RouterLink
          >
          and
          <RouterLink to="/teachers" class="link link-primary"
            >teachers</RouterLink
          >
          or you can get your
          <RouterLink to="/token" class="link link-primary"
            >JWT Token</RouterLink
          >
          and use it with an app such as
          <a
            class="link link-primary"
            target="__blank"
            href="https://insomnia.rest/"
            >insomnia</a
          >.
        </p>
        <h2>Authorization</h2>
        <p>You will need at least 2 accounts on your okta app</p>
        <ul>
          <li>
            <span class="underline decoration-primary">johndoe@gmail.com</span>
            to delete students or teachers
          </li>
          <li>
            <span class="underline decoration-primary">user@gmail.com</span> to
            create students or teachers
          </li>
        </ul>
        <p>Anyone can view students or teachers, even anonymous users.</p>
      </div>
      <div v-if="claimsStore.userClaims">
        <h1>Hello, {{ claimsStore.userClaims.name }}</h1>
        <p>
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
