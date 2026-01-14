package com.biometric.service;

import org.springframework.stereotype.Service;
import com.biometric.repository.FingerprintRepository;
import com.biometric.util.CryptoUtil;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import jakarta.annotation.PostConstruct;

@Service
public class TemplateCacheService {

    private final FingerprintRepository repo;
    // Changed from FingerprintTemplate to byte[] because R307 data is raw
    private final Map<String, byte[]> cache = new ConcurrentHashMap<>();

    public TemplateCacheService(FingerprintRepository repo) {
        this.repo = repo;
    }

    @PostConstruct
    public void loadAll() {
        int success = 0;
        int failed = 0;
        for (com.biometric.model.FingerprintTemplateEntity e : repo.findAll()) {
            try {
                // Decrypt the stored data
                byte[] decrypted = CryptoUtil.decrypt(e.getTemplate());
                cache.put(e.getName(), decrypted);
                success++;
            } catch (Exception ex) {
                System.err.println("❌ Failed to load template for " + e.getName() + ": " + ex.getMessage());
                failed++;
            }
        }
        System.out.println("✅ Cache initialized. Loaded: " + success + ", Failed: " + failed);
    }

    public void put(String name, byte[] rawTemplate) {
        cache.put(name, rawTemplate);
    }

    public Map<String, byte[]> all() {
        return cache;
    }
}