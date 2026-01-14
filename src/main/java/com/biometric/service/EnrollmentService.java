// ---EnrollmentService.java ----
@Service
public class EnrollmentService {

    private final FingerprintRepository repo;
    private final TemplateCacheService cache;

    public EnrollmentService(FingerprintRepository repo,
                             TemplateCacheService cache) {
        this.repo = repo;
        this.cache = cache;
    }

    public void enroll(String name, byte[] iso) throws Exception {

        FingerprintTemplate tpl = new FingerprintTemplate(iso);

        FingerprintTemplateEntity e = new FingerprintTemplateEntity();
        e.setName(name);
        e.setTemplate(CryptoUtil.encrypt(iso));

        repo.save(e);
        cache.put(name, tpl);

        System.out.println("Template saved for " + name);
    }
}
