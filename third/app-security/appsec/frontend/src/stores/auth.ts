import { watch, ref, getCurrentInstance } from "vue";
import { UserClaims, AuthState, OktaAuth } from "@okta/okta-auth-js";
import { defineStore } from "pinia";

export const useAuthStore = defineStore("authStore", () => {
  const instance = getCurrentInstance();
  const $auth: OktaAuth = ref(
    instance?.appContext.config.globalProperties.$auth
  );

  return {
    $auth,
  };
});

export const useAuthStateStore = defineStore("authState", () => {
  const authStore = useAuthStore();
  const authState = ref(authStore.$auth?.authStateManager.getAuthState());

  function handler(newState: AuthState) {
    authState.value = newState;
  }

  authStore.$auth.authStateManager.subscribe(handler);

  return {
    authState,
  };
});

export const useUserClaimsStore = defineStore("userClaims", () => {
  const authStore = useAuthStore();
  const authStateStore = useAuthStateStore();
  const userClaims = ref<UserClaims>();

  watch(
    () => authStateStore.authState.isAuthenticated,
    async (newV) => {
      if (newV === true) {
        userClaims.value = await authStore.$auth.getUser();
      }
    },
    { immediate: true, deep: true }
  );

  return {
    userClaims,
  };
});
