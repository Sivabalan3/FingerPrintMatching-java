// -----TemplateCacheService.java ---
@Service
public class TemplateCacheService {

    private final FingerprintRepository repo;
    private final Map<String, FingerprintTemplate> cache =
            new ConcurrentHashMap<>();

    public TemplateCacheService(FingerprintRepository repo) {
        this.repo = repo;
    }

    @PostConstruct
    public void loadAll() throws Exception {
        for (FingerprintTemplateEntity e : repo.findAll()) {
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
