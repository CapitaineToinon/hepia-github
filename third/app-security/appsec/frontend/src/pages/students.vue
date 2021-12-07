<script setup lang="ts">
import type { Student, SuccessResponse } from "../types";
import { ref } from "vue";
import Modal from "../components/Modal/Student.vue";
import { useApi } from "../composable/useApi";
import { useToggle } from "@vueuse/shared";
import { MenuIcon, TrashIcon } from "@heroicons/vue/solid";

const api = useApi();
const { data, isFetching, execute } =
  api("/students").json<SuccessResponse<Student[]>>();

const modal = ref(false);
const toggle = useToggle(modal);

function onStudentCreate() {
  execute();
}
</script>

<template>
  <div class="card bg-base-100 shadow-md rounded-md">
    <div class="card-body flex flex-col gap-3">
      <div class="flex justify-end">
        <button @click="() => toggle()" class="btn btn-primary">Add</button>
        <Modal v-model:visible="modal" @create="onStudentCreate" />
      </div>
      <table class="table table-compact w-full">
        <thead>
          <tr>
            <th>Name</th>
            <th>Last name</th>
            <th>Branch</th>
            <th><!-- empty --></th>
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
            <td>
              <button class="float-right">
                <TrashIcon class="h-6 w-6 text-error" />
              </button>
            </td>
          </tr>
        </tbody>
      </table>
    </div>
  </div>
</template>
