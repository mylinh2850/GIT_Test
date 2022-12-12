import i18n from "i18next";
import { initReactI18next } from "react-i18next";
import en from "./public/locales/en/translation.json";
import vi from "./public/locales/vi/translation.json";

i18n.use(initReactI18next).init({
  resources: {
    en: {
      translation: en,
    },
    vi: {
      translation: vi,
    },
  },
  fallbackLng: "en",
  interpolation: {
    escapeValue: false,
  },
});

export default i18n;