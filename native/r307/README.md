fingerprint-matcher/
│
├── pom.xml
├── README.md
│
├── native/                      👈 C++ (R307S)
│   ├── r307/
│   │   ├── r307_capture.cpp
│   │   ├── r307_protocol.h
│   │   └── README.md
│   │
│   └── build.sh
│
├── src/main/java/com/biometric/
│   │
│   ├── Application.java         👈 CLI (MODE 1 / MODE 2)
│   │
│   ├── config/
│   │   ├── SourceAfisConfig.java
│   │   └── ThreadPoolConfig.java
│   │
│   ├── service/
│   │   ├── EnrollmentService.java
│   │   ├── VerificationService.java
│   │   └── TemplateCacheService.java
│   │
│   ├── model/
│   │   └── FingerprintTemplateEntity.java
│   │
│   ├── repository/
│   │   └── FingerprintRepository.java
│   │
│   └── util/
│       ├── IsoTemplateUtil.java
│       └── CryptoUtil.java
│
└── src/main/resources/
    └── application.yml
