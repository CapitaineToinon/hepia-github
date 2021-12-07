module.exports = {
  mode: "jit",
  purge: ["./index.html", "./src/**/*.{vue,js,ts,jsx,tsx}"],
  darkMode: false, // using daisy for theme switching
  theme: {
    fontFamily: {
      sans: "Quicksand, Helvetica, Arial, sans-serif",
    },
  },
  variants: {
    extend: {},
  },
  daisyui: {
    themes: ["dark", "light"],
  },
  plugins: [require("@tailwindcss/typography"), require("daisyui")],
};
