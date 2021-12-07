<script setup lang="ts">
import type { Student, SuccessResponse } from "../types";
import { watchEffect, reactive, toRefs } from "vue";
import { reactivePick } from "@vueuse/core";
import { useApi } from "../composable/useApi";

const api = useApi();
const { data, isFetching } =
  api("/students").json<SuccessResponse<Student[]>>();

const lmao = toRefs(data);

watchEffect(() => console.log(lmao.value));
</script>

<template>
  <div class="container max-w-screen-md mx-auto">
    <div class="card shadow-md rounded-md">
      <div class="card-body">
        <table class="table w-full">
          <thead>
            <tr>
              <th>Name</th>
              <th>Last name</th>
              <th>Cursus</th>
            </tr>
          </thead>
          <tbody>
            <tr v-if="isFetching">
              <td class="text-center" colspan="3">Loading...</td>
            </tr>
            <tr v-if="data" v-for="student in data.data" :key="student.ID">
              <td>
                {{ student.name }}
              </td>
              <td>
                {{ student.lastname }}
              </td>
              <td>
                {{ student.filiere }}
              </td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>
  </div>
</template>
