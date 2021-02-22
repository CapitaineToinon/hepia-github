package app;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Properties {
    private HashMap<String, String> map = new HashMap<String, String>();

    public String getProperty(String key) {
        return map.get(key);
    }

    public String getPropertyOrElse(String key, String defaultKey) {
        String value = getProperty(key);
        return (value != null) ? value : getProperty(defaultKey);
    }

    public void addProperty(String key, String value) {
        map.put(key, value);
    }

    public List<String> keys() {
        return new ArrayList<String>(map.keySet());
    }

    public List<String> values() {
        return new ArrayList<String>(map.values());
    }

    public List<Map.Entry<String, String>> allProperties() {
        return new ArrayList<>(map.entrySet());
    }

    @Override
    public boolean equals(Object o) {
        if (this == o)
            return true;

        if (o == null || o.getClass() != this.getClass())
            return false;

        Properties that = (Properties) o;
        return map.equals(that.map);
    }

    @Override
    public int hashCode() {
        return map.hashCode();
    }

    @Override
    public String toString() {
        return String.format("Properties({})", map.toString());
    }
}