<script setup lang="ts">
import type { Teacher, SuccessResponse } from "../types";
import { ref } from "vue";
import Modal from "../components/Modal/Teacher.vue";
import EmtpyTableRow from "../components/EmtpyTableRow.vue";
import { useApi } from "../composable/useApi";
import { useToggle } from "@vueuse/shared";
import {
  TrashIcon,
  UserAddIcon,
  InformationCircleIcon,
} from "@heroicons/vue/solid";
import { useToast } from "../composable/useToast";
import { useAuthStore } from "../stores/auth";

const authState = useAuthStore();

const api = useApi();
const toast = useToast();
const { data, isFetching, execute, onFetchError } =
  api("/teachers").json<SuccessResponse<Teacher[]>>();

onFetchError(errorHandler);

const modal = ref(false);
const toggle = useToggle(modal);

function onTeacherCreate() {
  execute();
}

function tryDelete(id: number) {
  const { onFetchResponse: onDeleteResponse, onFetchError: onDeleteError } =
    api("/teachers").delete({
      id: id.toString(),
    });

  onDeleteError(errorHandler);

  onDeleteResponse(() => {
    toast.success("Teacher deleted");
    execute();
  });
}

function errorHandler(error: any) {
  toast.error(error);
}
</script>

<template>
  <div v-if="authState.isAuthenticated" class="flex justify-end">
    <button @click="() => toggle()" class="btn btn-primary flex gap-3">
      <UserAddIcon class="h-6 w-6" />Add Teacher
    </button>
    <Modal v-model:visible="modal" @create="onTeacherCreate" />
  </div>
  <div v-else class="alert alert-info">
    <div class="flex-1">
      <InformationCircleIcon class="h-6 w-6 mr-3" />
      <label
        >You need to
        <a class="link" @click="() => authState.$auth.signInWithRedirect()"
          >sign-in</a
        >
        to create or delete teachers.</label
      >
    </div>
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
          <template v-else>
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
                <button
                  v-if="authState.isAuthenticated"
                  @click="() => tryDelete(teacher.ID)"
                  class="float-right"
                >
                  <TrashIcon class="h-6 w-6 text-error" />
                </button>
              </td>
            </tr>
            <EmtpyTableRow v-if="!data?.data.length" colspan="4" />
          </template>
        </tbody>
      </table>
    </div>
  </div>
</template>
