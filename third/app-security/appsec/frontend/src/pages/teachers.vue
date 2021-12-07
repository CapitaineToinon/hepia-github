<script setup lang="ts">
import type { Teacher, SuccessResponse } from "../types";
import { useApi } from "../composable/useApi";

const api = useApi();
const { data } = api("/teachers").json<SuccessResponse<Teacher[]>>();
</script>

<template>
  <div v-if="data">
    <li v-for="teacher in data.data" :key="teacher.ID">
      {{ teacher.lastname }}
    </li>
  </div>
</template>
