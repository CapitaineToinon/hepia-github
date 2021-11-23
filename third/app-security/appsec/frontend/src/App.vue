<script setup lang="ts">
import { onMounted } from "vue";
import HelloWorld from './components/HelloWorld.vue'

onMounted(async () => {
  const response = await fetch('http://localhost:5000/students')

  if (response.status === 401) {
    window.location.href = 'http://localhost:3000/auth/login'
  }
})
</script>

<template>
  <img alt="Vue logo" src="./assets/logo.png" />
  <HelloWorld msg="Hello Vue 3 + TypeScript + Vite" />
</template>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
