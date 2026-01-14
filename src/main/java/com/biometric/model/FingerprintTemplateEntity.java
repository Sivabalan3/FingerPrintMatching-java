// ----FingerprintTemplateEntity.java -- 
@Document(collection = "fingerprints")
public class FingerprintTemplateEntity {

    @Id
    private String name;     // "siva"

    private byte[] template; // encrypted ISO

    private Instant createdAt = Instant.now();
}
