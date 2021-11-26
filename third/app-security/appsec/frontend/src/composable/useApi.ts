import { createFetch } from "@vueuse/core";
import { useAuthStore } from "../stores/auth";

export function useApi() {
  const { $auth } = useAuthStore();

  return createFetch({
    baseUrl: import.meta.env.DEV ? "http://localhost:3000/api" : "/api",
    options: {
      beforeFetch({ options }) {
        const myToken = $auth.getAccessToken();

        if (!myToken)
          $auth.signInWithRedirect({
            originalUri: window.location.href,
          });

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
