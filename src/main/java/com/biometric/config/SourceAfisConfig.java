package com.biometric.config;

import com.machinezoo.sourceafis.FingerprintMatcher;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class SourceAfisConfig {

    @Bean
    public FingerprintMatcher fingerprintMatcher() {
        // SourceAFIS 3.x does not support setThreshold()
        // Threshold logic will be applied manually in VerificationService
        return new FingerprintMatcher();
    }
}
