<script setup lang="ts">
import { useApi } from "../composable/useApi";

const api = useApi();
const { data } = api<any[]>("/teachers").get().json();
</script>

<template>
  <div v-if="data">
    <li v-for="student in data.data" :key="student.ID">
      {{ student.Lastname }}
    </li>
  </div>
</template>
