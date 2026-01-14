package com.biometric.service;

import com.biometric.model.FingerprintTemplateEntity;
import com.biometric.repository.FingerprintRepository;
import com.biometric.util.CryptoUtil;
import org.springframework.stereotype.Service;

@Service
public class EnrollmentService {

    private final FingerprintRepository repo;

    public EnrollmentService(FingerprintRepository repo) {
        this.repo = repo;
    }

    public void enroll(String name, byte[] iso) throws Exception {

        // Create entity
        FingerprintTemplateEntity e = new FingerprintTemplateEntity();
        e.setName(name);
        e.setTemplate(CryptoUtil.encrypt(iso)); // store encrypted ISO

        repo.save(e);

        System.out.println("Template saved for " + name);
    }
}
