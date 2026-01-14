package com.biometric.service;

import org.springframework.stereotype.Service;
import com.biometric.repository.FingerprintRepository;
import com.biometric.util.CryptoUtil;
import com.machinezoo.sourceafis.FingerprintTemplate;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import jakarta.annotation.PostConstruct;

@Service
public class TemplateCacheService {

    private final FingerprintRepository repo;
    private final Map<String, FingerprintTemplate> cache = new ConcurrentHashMap<>();

    public TemplateCacheService(FingerprintRepository repo) {
        this.repo = repo;
    }

    @PostConstruct
    public void loadAll() throws Exception {
        for (com.biometric.model.FingerprintTemplateEntity e : repo.findAll()) {
            byte[] iso = CryptoUtil.decrypt(e.getTemplate());
            cache.put(e.getName(), new FingerprintTemplate(iso));
        }
        System.out.println("Loaded " + cache.size() + " templates");
    }

    public void put(String name, FingerprintTemplate tpl) {
        cache.put(name, tpl);
    }

    public Map<String, FingerprintTemplate> all() {
        return cache;
    }
}
