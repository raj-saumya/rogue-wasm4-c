const { execSync } = require("child_process");
const fs = require("fs");

const templateContent = `{{#sprites}}
// {{name}}
#define {{name}}_W {{width}}
#define {{name}}_H {{height}}
#define {{name}}_F {{flagsHumanReadable}}
const uint8_t {{name}}[{{length}}] = { {{bytes}} };
{{/sprites}}`;

const templateFile = "util/sprite_template.mustache";
const imagePath = process.argv[2];

if (!imagePath) {
  console.error("Usage: node convert.js <path-to-image.png>");
  process.exit(1);
}

if (!fs.existsSync(templateFile)) {
  console.log(`Creating ${templateFile}...`);
  fs.writeFileSync(templateFile, templateContent);
}

try {
  console.log(`Converting ${imagePath}...`);

  // This runs the CLI command: w4 png2src --template ... <image>
  const output = execSync(
    `w4 png2src --template ${templateFile} "${imagePath}"`,
    {
      encoding: "utf-8",
    }
  );

  console.log("\n--- Generated C Code ---\n");
  console.log(output);
} catch (error) {
  console.error(error.message);
}
