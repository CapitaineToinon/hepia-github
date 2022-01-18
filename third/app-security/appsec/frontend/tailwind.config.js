module.exports = {
  content: ["./index.html", "./src/**/*.{vue,js,ts,jsx,tsx}"],
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
