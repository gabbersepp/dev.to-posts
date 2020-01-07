describe("suite", () => {
  it("this test should appear in the html and junit report", () => {
    expect(true).toBe(true)
  })

  it("Skipped", () => {
    expect(false).toBe(true)
  })
})
