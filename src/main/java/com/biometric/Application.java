package com.biometric;

import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

import com.biometric.service.EnrollmentService;
import com.biometric.service.VerificationService;
import com.machinezoo.sourceafis.FingerprintTemplate;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.Base64;
import java.util.Scanner;

@SpringBootApplication
public class Application implements CommandLineRunner {

    private final EnrollmentService enroll;
    private final VerificationService verify;

    public Application(EnrollmentService enroll,
                       VerificationService verify) {
        this.enroll = enroll;
        this.verify = verify;
    }

    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }

    @Override
    public void run(String... args) throws Exception {

        Scanner sc = new Scanner(System.in);
        System.out.println("1) Enroll\n2) Verify");
        int mode = sc.nextInt();
        sc.nextLine();

        if (mode == 1) {
            System.out.print("Enter name: ");
            String name = sc.nextLine();

            byte[] iso = captureFromR307("enroll");
            enroll.enroll(name, iso);

            System.out.println("Template saved for " + name);
        }

        if (mode == 2) {
            byte[] iso = captureFromR307("verify");

            String match = verify.verify(iso);
            if (match != null)
                System.out.println("Matched filename and name is " + match);
            else
                System.out.println("No match");
        }
    }

    private byte[] captureFromR307(String mode) throws Exception {

        Process p = new ProcessBuilder(
                "./native/r307/r307_capture", mode
        ).start();

        BufferedReader br = new BufferedReader(
                new InputStreamReader(p.getInputStream()));

        String line;
        while ((line = br.readLine()) != null) {
            if (line.startsWith("ISO_TEMPLATE_BASE64=")) {
                return Base64.getDecoder()
                        .decode(line.split("=")[1]);
            }
        }
        throw new RuntimeException("No template from scanner");
    }
}
