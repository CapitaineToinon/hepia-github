<script setup lang="ts">
import { ExclamationIcon } from "@heroicons/vue/outline";

defineProps<{
  colspan: string | number;
}>();
</script>

<template>
  <tr>
    <td :colspan="colspan" class="!px-0">
      <div class="alert alert-warning">
        <div class="flex-1">
          <ExclamationIcon class="h-6 w-6 mr-3" />
          <label>There are no entries.</label>
        </div>
      </div>
    </td>
  </tr>
</template>
