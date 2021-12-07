<script setup lang="ts">
import type { Student } from "../../types";
import { ref } from "vue";
import { useVModel, get, set } from "@vueuse/core";
import { useApi } from "../../composable/useApi";
import { useToast } from "../../composable/useToast";

const props = defineProps<{
  visible: boolean;
}>();

const emit = defineEmits(["update:visible", "create"]);

const api = useApi();
const { success, error } = useToast();
const open = useVModel(props, "visible");

const initial: Student = {
  lastname: "",
  name: "",
  filiere: "",
};

const student = ref<Student>({ ...initial });

const { post, isFetching } = api("/students", { immediate: false });

async function confirm() {
  try {
    await post(get(student)).execute(true);
    success("Student created");
    emit("create", get(student));
    close();
  } catch (e) {
    error((e as Error).message);
  }
}

function close() {
  set(open, false);
  set(student, { ...initial });
}
</script>

<template>
  <div class="modal z-40" :class="{ 'modal-open': open }">
    <div class="modal-box">
      <form class="flex flex-col gap-3">
        <div class="prose">
          <h2>Add a new student</h2>
        </div>
        <div class="form-control">
          <label class="label">
            <span class="label-text">Lastname</span>
          </label>
          <input
            v-model="student.lastname"
            type="text"
            placeholder="username"
            class="input input-bordered"
          />
        </div>
        <div class="form-control">
          <label class="label">
            <span class="label-text">Name</span>
          </label>
          <input
            v-model="student.name"
            type="text"
            placeholder="username"
            class="input input-bordered"
          />
        </div>
        <div class="form-control">
          <label class="label">
            <span class="label-text">Branch</span>
          </label>
          <input
            v-model="student.filiere"
            type="text"
            placeholder="username"
            class="input input-bordered"
          />
        </div>
      </form>

      <div class="modal-action">
        <button
          @click="close"
          for="my-modal-2"
          class="btn btn-ghost"
          :disabled="isFetching"
        >
          Cancel
        </button>
        <button
          @click="confirm"
          for="my-modal-2"
          class="btn btn-primary"
          :class="{ loading: isFetching }"
          :disabled="isFetching"
        >
          Confirm
        </button>
      </div>
    </div>
  </div>
</template>
