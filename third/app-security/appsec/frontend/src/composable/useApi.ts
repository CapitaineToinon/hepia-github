import { createFetch } from "@vueuse/core";
import { useAuthStore } from "../stores/auth";

export function useApi() {
  const authStore = useAuthStore();

  return createFetch({
    baseUrl: import.meta.env.DEV ? "http://localhost:3000/api" : "/api",
    options: {
      async beforeFetch({ options }) {
        const myToken = authStore.$auth.getAccessToken();

        if (!myToken) {
          try {
            await authStore.$auth.signInWithRedirect({
              originalUri: window.location.href,
            });
          } catch (e) {
            console.error(e);
          }
        }

        options.headers = {
          ...(options.headers ?? {}),
          Authorization: `Bearer ${myToken}`,
        };

        return { options };
      },
    },
    fetchOptions: {
      mode: "cors",
    },
  });
}
