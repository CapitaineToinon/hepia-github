import { createFetch } from "@vueuse/core";
import { useAuthStore } from "../stores/auth";

export function useApi() {
  const authStore = useAuthStore();

  return createFetch({
    baseUrl: import.meta.env.DEV
      ? `http://localhost:${import.meta.env.VITE_BACKEND}/api`
      : "/api",
    options: {
      async beforeFetch({ options }) {
        const myToken = authStore.$auth.getAccessToken();

        if (myToken) {
          options.headers = {
            ...(options.headers ?? {}),
            Authorization: `Bearer ${myToken}`,
          };
        }

        return { options };
      },
    },
    fetchOptions: {
      mode: "cors",
    },
  });
}
