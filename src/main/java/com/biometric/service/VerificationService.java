// -----VerificationService.java----
@Service
public class VerificationService {

    private final TemplateCacheService cache;
    private final FingerprintMatcher matcher =
            new FingerprintMatcher();

    public VerificationService(TemplateCacheService cache) {
        this.cache = cache;
    }

    public String verify(byte[] probeIso) {

        FingerprintTemplate probe =
                new FingerprintTemplate(probeIso);

        double best = 0;
        String bestName = null;

        for (var e : cache.all().entrySet()) {
            double score = matcher.match(probe, e.getValue());
            if (score > best) {
                best = score;
                bestName = e.getKey();
            }
        }
        return best > 40 ? bestName : null;
    }
}
