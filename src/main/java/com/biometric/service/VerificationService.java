package com.biometric.service;

import org.springframework.stereotype.Service;
import com.machinezoo.sourceafis.FingerprintMatcher;
import com.machinezoo.sourceafis.FingerprintTemplate;
import java.util.Map;

@Service
public class VerificationService {

    private final TemplateCacheService cache;
    private static final double MATCH_THRESHOLD = 40.0;

    public VerificationService(TemplateCacheService cache) {
        this.cache = cache;
    }

    public String verify(byte[] probeIso) {

        FingerprintTemplate probe = new FingerprintTemplate(probeIso);

        // Construct matcher with probe template
        FingerprintMatcher matcher = new FingerprintMatcher(probe);

        double bestScore = 0;
        String bestName = null;

        for (Map.Entry<String, FingerprintTemplate> entry : cache.all().entrySet()) {
            double score = matcher.match(entry.getValue());
            if (score > bestScore) {
                bestScore = score;
                bestName = entry.getKey();
            }
        }

        // compare score manually to threshold
        return bestScore >= MATCH_THRESHOLD ? bestName : null;
    }
}
