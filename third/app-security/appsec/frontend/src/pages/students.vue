<script setup lang="ts">
import { useApi } from "../composable/useApi";

const api = useApi();
const { data, isFetching } = api<any[]>("/students").get().json();
</script>

<template>
  <div class="container mx-auto">
    <div v-if="isFetching">Loading</div>
    <div v-if="data">
      <li v-for="student in data.data" :key="student.ID">
        {{ student.Lastname }}
      </li>
    </div>
  </div>
</template>
