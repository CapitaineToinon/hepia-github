<script setup lang="ts">
import type { Teacher, SuccessResponse } from "../types";
import { ref } from "vue";
import Modal from "../components/Modal/Teacher.vue";
import { useApi } from "../composable/useApi";
import { useToggle } from "@vueuse/shared";
import { TrashIcon, UserAddIcon } from "@heroicons/vue/solid";
import { useToast } from "../composable/useToast";

const api = useApi();
const toast = useToast();
const { data, isFetching, execute, onFetchError } =
  api("/teachers").json<SuccessResponse<Teacher[]>>();

const modal = ref(false);
const toggle = useToggle(modal);

function onTeacherCreate() {
  execute();
}

onFetchError((error) => {
  toast.error(error);
});
</script>

<template>
  <div class="flex justify-end">
    <button @click="() => toggle()" class="btn btn-primary flex gap-3">
      <UserAddIcon class="h-6 w-6" />Add Teacher
    </button>
    <Modal v-model:visible="modal" @create="onTeacherCreate" />
  </div>
  <div class="card bg-base-100 shadow-md rounded-md mt-3">
    <div class="card-body flex flex-col gap-3">
      <table class="table table-compact w-full">
        <thead>
          <tr>
            <th>Name</th>
            <th>Last name</th>
            <th>Class</th>
            <th><!-- empty --></th>
          </tr>
        </thead>
        <tbody>
          <tr v-if="isFetching">
            <td class="text-center" colspan="4">Loading...</td>
          </tr>
          <tr v-if="data" v-for="teacher in data.data" :key="teacher.ID">
            <td>
              {{ teacher.name }}
            </td>
            <td>
              {{ teacher.lastname }}
            </td>
            <td>
              {{ teacher.class }}
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
