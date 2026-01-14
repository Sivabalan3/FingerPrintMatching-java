package com.biometric.repository;

import com.biometric.model.FingerprintTemplateEntity;
import org.springframework.data.mongodb.repository.MongoRepository;
import org.springframework.stereotype.Repository;

/**
 * Spring Data MongoDB repository for storing fingerprint templates.
 * The entity is FingerprintTemplateEntity and the ID type is String (name).
 */
@Repository
public interface FingerprintRepository extends MongoRepository<FingerprintTemplateEntity, String> {
    // You can add custom query methods here if needed, e.g.:
    // Optional<FingerprintTemplateEntity> findByName(String name);
}
