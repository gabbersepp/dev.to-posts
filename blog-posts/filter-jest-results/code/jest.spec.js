describe("suite", () => {
  it("this test should appear in the html and junit report", () => {
    expect(true).toBe(true)
  })

  it("this test should not appear in the html and junit report", () => {
    expect(false).toBe(true)
  })
})
