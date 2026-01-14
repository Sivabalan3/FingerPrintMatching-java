package com.biometric.service;

import com.biometric.model.FingerprintTemplateEntity;
import com.biometric.repository.FingerprintRepository;
import com.biometric.util.CryptoUtil;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class VerificationService {

    private final FingerprintRepository repo;
    private static final double MATCH_THRESHOLD = 0.95; // example for similarity

    public VerificationService(FingerprintRepository repo) {
        this.repo = repo;
    }

    public String verify(byte[] probeIso) throws Exception {

        List<FingerprintTemplateEntity> all = repo.findAll();

        for (FingerprintTemplateEntity e : all) {
            byte[] storedIso = CryptoUtil.decrypt(e.getTemplate());

            // simple equality check for demo (replace with your matcher)
            if (java.util.Arrays.equals(probeIso, storedIso)) {
                return e.getName();
            }
        }

        return null; // no match
    }
}
